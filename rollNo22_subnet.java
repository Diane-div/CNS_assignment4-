import java.util.*;

public class rollNo22_subnet {

    // Convert a byte to 8-bit binary string
    private static String toBinaryString(int num) {
        return String.format("%8s", Integer.toBinaryString(num)).replace(' ', '0');
    }

    // Convert dotted-decimal IP to binary
    private static String ipToBinary(String ip) {
        String[] parts = ip.split("\\.");
        StringBuilder binaryIP = new StringBuilder();
        for (String part : parts) {
            binaryIP.append(toBinaryString(Integer.parseInt(part))).append(" ");
        }
        return binaryIP.toString().trim();
    }

    // Calculate new CIDR prefix
    private static int calculateNewPrefix(int originalPrefix, int requiredSubnets) {
        int bitsNeeded = (int) Math.ceil(Math.log(requiredSubnets) / Math.log(2));
        return originalPrefix + bitsNeeded;
    }

    // Convert CIDR prefix to dotted decimal subnet mask
    private static String cidrToSubnetMask(int cidr) {
        int mask = 0xffffffff << (32 - cidr);
        int octet1 = (mask >>> 24) & 0xff;
        int octet2 = (mask >>> 16) & 0xff;
        int octet3 = (mask >>> 8) & 0xff;
        int octet4 = mask & 0xff;
        return octet1 + "." + octet2 + "." + octet3 + "." + octet4;
    }

    // Convert dotted-decimal IP to integer
    private static int ipToInt(String ip) {
        String[] parts = ip.split("\\.");
        int result = 0;
        for (int i = 0; i < 4; i++) {
            result |= Integer.parseInt(parts[i]) << (24 - (8 * i));
        }
        return result;
    }

    // Convert integer to dotted-decimal IP
    private static String intToIp(int ip) {
        return ((ip >> 24) & 0xff) + "." +
               ((ip >> 16) & 0xff) + "." +
               ((ip >> 8) & 0xff) + "." +
               (ip & 0xff);
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Input IP Address
        System.out.print("Enter IP address (e.g., 192.168.1.0): ");
        String ipAddress = scanner.nextLine();

        // Input CIDR prefix
        System.out.print("Enter network mask (CIDR format, e.g., 24): ");
        int cidr = scanner.nextInt();

        // Display binary forms
        System.out.println("\n--- Binary Representation ---");
        System.out.println("IP Address (Binary):     " + ipToBinary(ipAddress));
        System.out.println("Subnet Mask (Binary):    " + ipToBinary(cidrToSubnetMask(cidr)));

        // Input number of required subnets
        System.out.print("\nEnter number of subnets required: ");
        int requiredSubnets = scanner.nextInt();

        // Calculate new CIDR and Subnet Mask
        int newCIDR = calculateNewPrefix(cidr, requiredSubnets);
        String newSubnetMask = cidrToSubnetMask(newCIDR);

        // Display result
        System.out.println("\n--- Subnetting Result ---");
        System.out.println("New CIDR Notation: /" + newCIDR);
        System.out.println("New Subnet Mask:   " + newSubnetMask);
        System.out.println("Subnet Mask (Binary): " + ipToBinary(newSubnetMask));

        // Calculate and display first 6 subnet IP address ranges
        System.out.println("\n--- First 6 Subnet Address Ranges ---");

        int baseIp = ipToInt(ipAddress);
        int numberOfSubnets = (int) Math.pow(2, newCIDR - cidr);
        int totalHosts = (int) Math.pow(2, 32 - newCIDR);
        int maxSubnetsToShow = Math.min(6, numberOfSubnets);

        for (int i = 0; i < maxSubnetsToShow; i++) {
            int networkAddress = baseIp + (i * totalHosts);
            int firstHost = networkAddress + 1;
            int lastHost = networkAddress + totalHosts - 2;
            int broadcastAddress = networkAddress + totalHosts - 1;

            System.out.println("\nSubnet " + (i + 1) + ":");

            System.out.println("Network Address:   " + intToIp(networkAddress));
            System.out.println("Binary:            " + ipToBinary(intToIp(networkAddress)));

            System.out.println("First Host:        " + intToIp(firstHost));
            System.out.println("Binary:            " + ipToBinary(intToIp(firstHost)));

            System.out.println("Last Host:         " + intToIp(lastHost));
            System.out.println("Binary:            " + ipToBinary(intToIp(lastHost)));

            System.out.println("Broadcast Address: " + intToIp(broadcastAddress));
            System.out.println("Binary:            " + ipToBinary(intToIp(broadcastAddress)));
        }

        scanner.close();
    }
}
